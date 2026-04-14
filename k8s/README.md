# Kubernetes Deployment Guide

This directory contains Kubernetes manifests for deploying all four secure containerized applications.

## 📋 Contents

- `go-app-deployment.yaml` - Go application deployment
- `cpp-app-deployment.yaml` - C++ application deployment
- `python-app-deployment.yaml` - Python application deployment
- `nodejs-app-deployment.yaml` - Node.js application deployment
- `ingress.yaml` - Ingress configuration for external access
- `network-policy.yaml` - Network policies for security
- `README.md` - This file

## 🚀 Quick Start

### Prerequisites

- Kubernetes cluster (v1.24+)
- kubectl configured
- Docker images built and available
- NGINX Ingress Controller (for ingress)

### Deploy All Applications

```bash
# Create namespace and deploy all applications
kubectl apply -f go-app-deployment.yaml
kubectl apply -f cpp-app-deployment.yaml
kubectl apply -f python-app-deployment.yaml
kubectl apply -f nodejs-app-deployment.yaml

# Optional: Deploy ingress
kubectl apply -f ingress.yaml

# Optional: Apply network policies
kubectl apply -f network-policy.yaml
```

### Verify Deployments

```bash
# Check all resources
kubectl get all -n secure-apps

# Check pods
kubectl get pods -n secure-apps

# Check services
kubectl get svc -n secure-apps

# Check ingress
kubectl get ingress -n secure-apps
```

## 📦 Individual Application Deployment

### Go Application

```bash
kubectl apply -f go-app-deployment.yaml

# Verify
kubectl get pods -n secure-apps -l app=secure-go-app
kubectl logs -n secure-apps -l app=secure-go-app

# Port forward for testing
kubectl port-forward -n secure-apps svc/secure-go-app 8080:80

# Test
curl http://localhost:8080/health
```

### C++ Application

```bash
kubectl apply -f cpp-app-deployment.yaml

# Verify
kubectl get pods -n secure-apps -l app=secure-cpp-app

# Port forward
kubectl port-forward -n secure-apps svc/secure-cpp-app 8081:80

# Test
curl http://localhost:8081/health
```

### Python Application

```bash
kubectl apply -f python-app-deployment.yaml

# Verify
kubectl get pods -n secure-apps -l app=secure-python-app

# Port forward
kubectl port-forward -n secure-apps svc/secure-python-app 8082:80

# Test
curl http://localhost:8082/health
```

### Node.js Application

```bash
kubectl apply -f nodejs-app-deployment.yaml

# Verify
kubectl get pods -n secure-apps -l app=secure-nodejs-app

# Port forward
kubectl port-forward -n secure-apps svc/secure-nodejs-app 8083:80

# Test
curl http://localhost:8083/health
```

## 🔒 Security Features

### Pod Security

All deployments include:

- ✅ **Non-root user** - Runs as UID 1001 or 65532
- ✅ **Read-only root filesystem** - Prevents tampering
- ✅ **No privilege escalation** - Enhanced security
- ✅ **Dropped capabilities** - Minimal permissions
- ✅ **Seccomp profile** - Runtime security
- ✅ **Resource limits** - Prevents resource exhaustion

### Network Security

- Network policies restrict traffic
- Services use ClusterIP (internal only)
- Ingress with TLS support
- Rate limiting configured

### Secrets Management

```bash
# Update license keys
kubectl create secret generic go-app-secrets \
  --from-literal=LICENSE_KEY=your-license-key \
  --namespace=secure-apps \
  --dry-run=client -o yaml | kubectl apply -f -
```

## 📊 Resource Requirements

| Application | CPU Request | CPU Limit | Memory Request | Memory Limit |
|-------------|-------------|-----------|----------------|--------------|
| Go | 10m | 100m | 16Mi | 64Mi |
| C++ | 20m | 200m | 32Mi | 128Mi |
| Python | 50m | 500m | 64Mi | 256Mi |
| Node.js | 30m | 300m | 48Mi | 192Mi |

## 🔄 Horizontal Pod Autoscaling

All applications include HPA configuration:

- **Min replicas**: 3
- **Max replicas**: 10
- **CPU target**: 70%
- **Memory target**: 80%

View HPA status:

```bash
kubectl get hpa -n secure-apps
```

## 🌐 Ingress Configuration

### Path-based Routing

Access applications via:
- `http://secure-apps.example.com/go`
- `http://secure-apps.example.com/cpp`
- `http://secure-apps.example.com/python`
- `http://secure-apps.example.com/nodejs`

### Subdomain-based Routing

Access applications via:
- `http://go.secure-apps.example.com`
- `http://cpp.secure-apps.example.com`
- `http://python.secure-apps.example.com`
- `http://nodejs.secure-apps.example.com`

### Enable TLS

1. Install cert-manager:
```bash
kubectl apply -f https://github.com/cert-manager/cert-manager/releases/download/v1.13.0/cert-manager.yaml
```

2. Create ClusterIssuer:
```yaml
apiVersion: cert-manager.io/v1
kind: ClusterIssuer
metadata:
  name: letsencrypt-prod
spec:
  acme:
    server: https://acme-v02.api.letsencrypt.org/directory
    email: your-email@example.com
    privateKeySecretRef:
      name: letsencrypt-prod
    solvers:
    - http01:
        ingress:
          class: nginx
```

3. Uncomment TLS section in ingress.yaml

## 🔍 Monitoring and Logging

### View Logs

```bash
# All pods in namespace
kubectl logs -n secure-apps -l app=secure-go-app --tail=100

# Follow logs
kubectl logs -n secure-apps -l app=secure-go-app -f

# Previous container logs
kubectl logs -n secure-apps <pod-name> --previous
```

### Check Pod Status

```bash
# Describe pod
kubectl describe pod -n secure-apps <pod-name>

# Get events
kubectl get events -n secure-apps --sort-by='.lastTimestamp'

# Check resource usage
kubectl top pods -n secure-apps
```

## 🧪 Testing

### Health Checks

```bash
# Test all health endpoints
for app in go cpp python nodejs; do
  echo "Testing $app..."
  kubectl run test-$app --rm -it --restart=Never --image=curlimages/curl -- \
    curl -s http://secure-$app-app/health
done
```

### Load Testing

```bash
# Install hey (HTTP load generator)
# https://github.com/rakyll/hey

# Port forward
kubectl port-forward -n secure-apps svc/secure-go-app 8080:80 &

# Run load test
hey -n 10000 -c 100 http://localhost:8080/health
```

## 🔧 Troubleshooting

### Pods Not Starting

```bash
# Check pod status
kubectl get pods -n secure-apps

# Describe pod
kubectl describe pod -n secure-apps <pod-name>

# Check events
kubectl get events -n secure-apps

# Check logs
kubectl logs -n secure-apps <pod-name>
```

### Image Pull Errors

```bash
# Check if images exist
docker images | grep secure-

# Tag images for registry
docker tag secure-go-app:latest your-registry/secure-go-app:latest

# Push to registry
docker push your-registry/secure-go-app:latest

# Update deployment to use registry
kubectl set image deployment/secure-go-app \
  go-app=your-registry/secure-go-app:latest \
  -n secure-apps
```

### Service Not Accessible

```bash
# Check service
kubectl get svc -n secure-apps

# Check endpoints
kubectl get endpoints -n secure-apps

# Test from within cluster
kubectl run test --rm -it --restart=Never --image=curlimages/curl -- \
  curl -v http://secure-go-app.secure-apps.svc.cluster.local/health
```

## 🧹 Cleanup

### Delete All Resources

```bash
# Delete all applications
kubectl delete -f go-app-deployment.yaml
kubectl delete -f cpp-app-deployment.yaml
kubectl delete -f python-app-deployment.yaml
kubectl delete -f nodejs-app-deployment.yaml
kubectl delete -f ingress.yaml
kubectl delete -f network-policy.yaml

# Or delete entire namespace
kubectl delete namespace secure-apps
```

### Delete Individual Application

```bash
kubectl delete -f go-app-deployment.yaml
```

## 📈 Scaling

### Manual Scaling

```bash
# Scale deployment
kubectl scale deployment secure-go-app --replicas=5 -n secure-apps

# Check status
kubectl get deployment secure-go-app -n secure-apps
```

### Update HPA

```bash
# Edit HPA
kubectl edit hpa secure-go-app-hpa -n secure-apps

# Or apply updated manifest
kubectl apply -f go-app-deployment.yaml
```

## 🔄 Rolling Updates

### Update Image

```bash
# Update image
kubectl set image deployment/secure-go-app \
  go-app=secure-go-app:v2 \
  -n secure-apps

# Check rollout status
kubectl rollout status deployment/secure-go-app -n secure-apps

# View rollout history
kubectl rollout history deployment/secure-go-app -n secure-apps
```

### Rollback

```bash
# Rollback to previous version
kubectl rollout undo deployment/secure-go-app -n secure-apps

# Rollback to specific revision
kubectl rollout undo deployment/secure-go-app --to-revision=2 -n secure-apps
```

## 🎯 Best Practices

1. **Use namespaces** - Isolate applications
2. **Set resource limits** - Prevent resource exhaustion
3. **Enable health checks** - Ensure reliability
4. **Use HPA** - Auto-scale based on load
5. **Apply network policies** - Restrict traffic
6. **Use secrets** - Never hardcode credentials
7. **Enable TLS** - Encrypt traffic
8. **Monitor logs** - Track application behavior
9. **Regular updates** - Keep images current
10. **Backup configs** - Version control manifests

## 📚 Additional Resources

- [Kubernetes Documentation](https://kubernetes.io/docs/)
- [NGINX Ingress Controller](https://kubernetes.github.io/ingress-nginx/)
- [cert-manager](https://cert-manager.io/)
- [Kubernetes Security Best Practices](https://kubernetes.io/docs/concepts/security/)

## 🆘 Support

For issues or questions:
- Check main [README.md](../README.md)
- Review [Documentation](../Docs/)
- Check [BUILD-VERIFICATION.md](../BUILD-VERIFICATION.md)